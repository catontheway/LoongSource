
FDGJ_TimerMin = 0  --��¼����е�ʱ��

FDGJ_KillGhost = 0 --��¼��б�ɱ����ԩ���ι����

LingHuo_Pos = {}    --��¼ˢ������λ������
LingHuo_Pos[1] = {x=1138, y=20139, z=822}
LingHuo_Pos[2] = {x=972, y=18885, z=1196}
LingHuo_Pos[3] = {x=963, y=23066, z=1807}
LingHuo_Pos[4] = {x=775, y=25000, z=1656}
LingHuo_Pos[5] = {x=1306, y=26047, z=1593}
LingHuo_Pos[6] = {x=1873, y=17854, z=1938}
LingHuo_Pos[7] = {x=1883, y=16255, z=1773}
LingHuo_Pos[8] = {x=2507, y=19760, z=339}
LingHuo_Pos[9] = {x=1689, y=19637, z=506}
LingHuo_Pos[10] = {x=1883, y=19667, z=1286}

LingHuo_Compensate = {}    --������¼��������Ѫ���Ƿ񱻶����1.


function FDGJ_OnStart(actID)
    --��ʼ���ȫ�ֱ���
	for k=1, 10 do
		table.remove(LingHuo_Compensate)
	end

	FDGJ_TimerMin = 0

	FDGJ_KillGhost = 0

	--������ȫ��ͨ�棺����ˮ��ۺ����ڡ���Ѿ���ʼ������ظ�֮�Ŵ󿪣�����Ѱ�ҳ�������ˮ�����ڵ�ͨڤ��𣬷���ڤǮ�Լ��Ϲ��񣡡�
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 718)
	msg.DispatchMapMsgEvent(MsgID, 3017299919, -1)

	--����ˮ�����ڵĲ߻��༭�ƶ��ص�ˢ�³�10��������̬NPC��ͨڤ���
	for i=1, 10 do  --���ɹ���
		local CreatureID = map.MapCreateCreature(3017299919, -1, 3021004, LingHuo_Pos[i].x, LingHuo_Pos[i].y, LingHuo_Pos[i].z)
		table.insert(LingHuo_Compensate, CreatureID, 0)
	end
end

function FDGJ_OnTimerMin(actID)

	FDGJ_TimerMin = FDGJ_TimerMin + 1

	if FDGJ_TimerMin == 5 then
	    --������ȫ��ͨ�棺����ˮ��ۺ����ڡ���Ѿ���ʼ������ظ�֮�Ŵ󿪣�����Ѱ�ҳ�������ˮ�����ڵ�ͨڤ��𣬷���ڤǮ�Լ��Ϲ��񣡡�
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 100, 718)
	    msg.DispatchMapMsgEvent(MsgID, 3017299919, -1)

	elseif FDGJ_TimerMin % 10 == 9 then
		--������ȫ��ͨ�棺��ͨڤ�����һ���Ӻ�ˢ�¡�
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 100, 719)
	    msg.DispatchMapMsgEvent(MsgID, 3017299919, -1)

	elseif FDGJ_TimerMin % 10 == 0 and FDGJ_TimerMin ~= 0 and FDGJ_TimerMin ~= 60 then
	    --��ʼ���ȫ�ֱ���
	    for k=1, 10 do
		    table.remove(LingHuo_Compensate)
	    end
	    for i=1, 10 do    --����ˮ�����ڵĲ߻��༭�ƶ��ص�ˢ�³�10��������̬NPC��ͨڤ���
			local CreatureID = map.MapCreateCreature(3017299919, -1, 3021004, LingHuo_Pos[i].x, LingHuo_Pos[i].y, LingHuo_Pos[i].z)
			table.insert(LingHuo_Compensate, CreatureID, 0)
	    end
	end
end


--ע��û������¼�
aux.RegisterActEvent(65, 2, "FDGJ_OnStart")
aux.RegisterActEvent(66, 2, "FDGJ_OnStart")
aux.RegisterActEvent(67, 2, "FDGJ_OnStart")
aux.RegisterActEvent(68, 2, "FDGJ_OnStart")

aux.RegisterActEvent(65, 4, "FDGJ_OnTimerMin")
aux.RegisterActEvent(66, 4, "FDGJ_OnTimerMin")
aux.RegisterActEvent(67, 4, "FDGJ_OnTimerMin")
aux.RegisterActEvent(68, 4, "FDGJ_OnTimerMin")

--����ͨڤ���
function C3021004_OnInVest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
    --�����ҵ���55�����������Ч
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level < 55 then
	    --���������Ѫ����
    	local HP = unit.GetUnitAttValue(MapID, InstanceID, CreatureID, 9)
		if HP == 3 then
		    LingHuo_Compensate[CreatureID] = 1     --��¼�õ���Ӧ�û��Ѫ������
		end
		--Ѫ������
		unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
	    --��ʾ�������ĵȼ�����55�������ܲμ�ۺ����ڻ��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 720)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
    	--�������Ƿ���ڤǮ��ֽ���������ֱ�ӿ۳�
    	local Role_Have_HuangZhi = role.GetRoleItemNum(RoleID, 2615901)
        --���û�л�ֽ
    	if Role_Have_HuangZhi < 1 or Role_Have_HuangZhi == nil then
    		--���������Ѫ����
        	local HP = unit.GetUnitAttValue(MapID, InstanceID, CreatureID, 9)
    		if HP == 3 then
    		    LingHuo_Compensate[CreatureID] = 1     --��¼�õ���Ӧ�û��Ѫ������
    		end
    		--Ѫ������
    		unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
    		--��ʾ������û��ڤǮ��ֽ���գ���׼������������
    		local MsgID = msg.BeginMsgEvent()
    		msg.AddMsgEvent(MsgID, 26, 721)
    		msg.DispatchRoleMsgEvent(RoleID, MsgID)
        else
	    	--��������пɹ�ɾ���Ļ�ֽ
	        if 0 == role.RemoveFromRole(MapID, InstanceID, RoleID, 2615901, 1, 1000) then
	    	    --��ʾ��������һ��ڤǮ��ֽ������֮�У�˲�仯Ϊ�ҽ�����
	    	    local MsgID = msg.BeginMsgEvent()
	    	    msg.AddMsgEvent(MsgID, 26, 722)
	    	    msg.DispatchRoleMsgEvent(RoleID, MsgID)

	    	    --������Ӧ�Ľ���
	    		local addexp = {}
	    		addexp[11] = 8380
	    		addexp[12] = 9136
	    		addexp[13] = 10116
	     		addexp[14] = 11916
	    		addexp[15] = 13761
	    		addexp[16] = 15641
	    		addexp[17] = 17841
	    		addexp[18] = 19641
	    		addexp[19] = 21044
				addexp[20] = 22044
				addexp[21] = 23044

	    		local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	    		local k = math.floor((rolelevel-1)/5)
	    		if rolelevel == 55 then
	    		    k = 11
	    		end
	    		role.AddRoleExp(MapID, InstanceID, RoleID, addexp[k])

		    	--��ʾ�����������XX�㾭�顱
				local MsgID = msg.BeginMsgEvent()
	    	    msg.AddMsgEvent(MsgID, 26, 723)
	    		msg.AddMsgEvent(MsgID, 9, addexp[k])
	     	    msg.DispatchRoleMsgEvent(RoleID, MsgID)

		    	--�����жϸ���Ѫ������
	            local HP = unit.GetUnitAttValue(MapID, InstanceID, CreatureID, 9)
	    	    if HP == 2 then
		            if LingHuo_Compensate[CreatureID] == 1 then       --�жϸõ���Ӧ�û��Ѫ������
			            --Ѫ������
		                unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
			    		LingHuo_Compensate[CreatureID] = 0     --���Ѫ������λ
					end
				end
		    else
		        --���������Ѫ����
				local HP = unit.GetUnitAttValue(MapID, InstanceID, CreatureID, 9)
		        if HP == 3 then
		            LingHuo_Compensate[CreatureID] = 1     --��¼�õ���Ӧ�û��Ѫ������
		        end
		        --Ѫ������
		        unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
		        --��ʾ������û��ڤǮ��ֽ���գ���׼������������
		        local MsgID = msg.BeginMsgEvent()
		        msg.AddMsgEvent(MsgID, 26, 721)
		        msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    end
	    end
	end
end
--ע�����ͨڤ����¼�
aux.RegisterCreatureEvent(3021004, 6, "C3021004_OnInVest")

--ͨڤ��𱻵�����ʧ��ˢ��3��ԩ���ι�1532041
function c3021004_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    local a, b, c = unit.GetPosition(MapID, InstanceID, TargetID)  --��ȡ��������
	map.MapCreateColCreature(MapID, InstanceID, 1532041, a, b, c, 1, "")
	map.MapCreateColCreature(MapID, InstanceID, 1532041, a, b, c, 1, "")
	map.MapCreateColCreature(MapID, InstanceID, 1532041, a, b, c, 1, "")
end
aux.RegisterCreatureEvent(3021004, 4, "c3021004_OnDie")

--ɱ��150��ԩ���ι����ˢ���޼�����1532042
function c1532041_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    FDGJ_KillGhost = FDGJ_KillGhost + 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 726)    --�Ѿ���ɱԩ���ι�XX�����ﵽ150���Ժ��޼����޽������
	msg.AddMsgEvent(MsgID, 9, FDGJ_KillGhost)
	msg.DispatchMapMsgEvent(MsgID, MapID, -1)
	if FDGJ_KillGhost == 150 then
        map.MapCreateCreature(MapID, InstanceID, 1532042, 977, 20140, 979)
    	--������ȫ��ͨ�棺���������걻���������ڣ��޼������ѱ����������Խ�����ˮ�л����ǣ����·Ӣ������ǰ����������
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 100, 724)
	    msg.DispatchMapMsgEvent(MsgID, 3017299919, -1)
    end
end
aux.RegisterCreatureEvent(1532041, 4, "c1532041_OnDie")

--ɱ���޼����ޣ�ȫ��ͨ�桰####�����޼�����������һ������ҽ�ɫ����Ӣ����˫�������޼����ޣ�ʵ������Ӣ��֮��ģ����
function c1532042_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    --֪ͨȫ��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 725)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)

	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 100, )
	--msg.AddMsgEvent(MsgID, 2, RoleID)
	--msg.DispatchWorldMsgEvent(MsgID)
end
aux.RegisterCreatureEvent(1532042, 4, "c1532042_OnDie")

-- ԩ���ι����ս������
function c1532041_OnEnterCombat(MapID, InstanceID, CreatureID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���˭�ܾ�������ຣ����
		cre.MonsterSay(MapID, InstanceID, CreatureID, 30140)
	end
end

aux.RegisterCreatureEvent(1532041, 2, "c1532041_OnEnterCombat")

-- �޼����޽���ս������
function c1532042_OnEnterCombat(MapID, InstanceID, CreatureID)
    --����˳���߲���������������
	cre.MonsterSay(MapID, InstanceID, CreatureID, 30141)
end

aux.RegisterCreatureEvent(1532042, 2, "c1532042_OnEnterCombat")
