LingPaiLingQu_Update = {}
--���ɻ
fengxiangjiaofei_man = {}
fengxiangjiaofei_kill = {}
fabu_date = 0
bangpaihuwei_instance = {}
function chengwufaburen(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local curhour = tonumber(os.date("%H"))
	if TalkIndex == -1 then
	    local Role_GuildID = guild.GetRoleGuildID(RoleID)
		local Master_GuilID = city.GetCityAppointedAtt(3, 2)
		local Role_ZhiWei = -1
		if Role_GuildID ~= nil and Role_GuildID ~= 4294967295 then
		    Role_ZhiWei = guild.GetRoleGuildPos(Role_GuildID, RoleID)
		end
	    if city.GetCityAppointedAtt(3, 2) == 4294967295 or city.GetCityAppointedAtt(3, 2) == nil then
			if Role_ZhiWei ~= 2 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3401)		--�����񷢲��ˣ�\n    �µĳ�����δ��������ʱ�������κ����񷢲�����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	21)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3407)		--�����񷢲��ˣ�\n    �����Ѿ��´�����˷��������С�������Ϊһ��֮������ÿ���������������ȡһ����˷�����ڷ��賡���ķ��踮��ʹ�ø����ƿ���Ϊ���ɳ�Ա����һ��������������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				if curhour >= 12 then
					msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	3406)			--����ȡ��˷����
				end
				msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif Role_GuildID==4294967295 or Role_ZhiWei ~= 2 then
		    if act.GetActScriptData(99, 1, 16) > 0 and role.GetRoleScriptData(RoleID, 1, RoleDataType["ChengZhuJingYan_Update"]) ~= tonumber(os.date("%j")) and curhour >= 18 and curhour <= 20 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3402)		--�����񷢲��ˣ�\n    �����������ʩ�������£������Դ���������ȡһ�����ľ��顣��ֻ��Ҫ�����л�����ܻ�ý�������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	3403)			--����л�������䡱
				msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3404)		--�����񷢲��ˣ�\n    �ܱ�Ǹ������û��������ص����񷢲�����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	21)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif Role_GuildID ~= -1 and Role_GuildID ~= 4294967295 and Role_GuildID ~= Master_GuilID and Role_ZhiWei == 2 then
		    if act.GetActScriptData(99, 1, 16) > 0 and role.GetRoleScriptData(RoleID, 1, RoleDataType["ChengZhuJingYan_Update"]) ~= tonumber(os.date("%j")) and curhour >= 18 and curhour <= 20 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3405)		--�����񷢲��ˣ�\n    �����������ʩ�������£������Դ���������ȡһ�����ľ��顣��ֻ��Ҫ�����л�����ܻ�ý�����\n    ���⣬�����Ѿ��´�����˷��������С�������Ϊһ��֮������ÿ���������������ȡһ����˷�����ڷ��踮��ʹ�ø����ƿ���Ϊ���ɳ�Ա����һ��������������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				--if role.GetRoleScriptData(RoleID, 1, RoleDataType["LingPaiLingQu_Update"]) ~= tonumber(os.date("%j")) then
				if curhour >= 12 then
					msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	3406)			--����ȡ��˷����
				end
				--end
				msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	3403)			--����л�������䡱
				msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
                local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3407)		--�����񷢲��ˣ�\n    �����Ѿ��´�����˷��������С�������Ϊһ��֮������ÿ���������������ȡһ����˷�����ڷ��賡���ķ��踮��ʹ�ø����ƿ���Ϊ���ɳ�Ա����һ��������������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				if curhour >= 12 then
					msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	3406)			--����ȡ��˷����
				end
				msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif Role_GuildID ~= -1 and Role_GuildID ~= 4294967295 and Role_GuildID == Master_GuilID and Role_ZhiWei == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	3408)		--�����񷢲��ˣ�\n    �������ã����뷢��һЩʲô�����أ�����˷�����ÿ��ֻ����ȡһ�Σ���ȡ֮������ڷ��賡���еķ��踮��Χ��ʹ��Ϊ���ɳ�Ա����һ����������������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			if curhour >= 12 then
				msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	3406)			--����ȡ��˷����
			end
			msg.AddMsgEvent(MsgID, 21,	6)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	3409)			--���������¡�
			msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	elseif TalkIndex == 4 then
        if act.GetActScriptData(99, 1, 16) > 0 then
		    if role.GetRoleScriptData(RoleID, 1, RoleDataType["ChengZhuJingYan_Update"]) ~= tonumber(os.date("%j")) then
			    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
                role.AddRoleExp(MapID, InstanceID, RoleID, math.floor(activity_newexp[rolelevel]/6))
				role.SetRoleScriptData(RoleID, 1, RoleDataType["ChengZhuJingYan_Update"], tonumber(os.date("%j")))
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3410)		--�����񷢲��ˣ�\n    ���Ѿ���ȡ���յĳ����ʹ;��顣��
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	21)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 5 then
	    local Role_GuildID = guild.GetRoleGuildID(RoleID)
		local Master_GuilID = city.GetCityAppointedAtt(3, 2)
		local Role_ZhiWei = -1
		if Role_GuildID ~= nil and Role_GuildID ~= 4294967295 then
		    Role_ZhiWei = guild.GetRoleGuildPos(Role_GuildID, RoleID)
		end
		if Role_GuildID ~= -1 and Role_GuildID ~= 4294967295 and Role_ZhiWei == 2 then

			if LingPaiLingQu_Update[Role_GuildID]==nil then
				LingPaiLingQu_Update[Role_GuildID]=0
			end

			if LingPaiLingQu_Update[Role_GuildID] ~= tonumber(os.date("%j")) then
			    if role.GetBagFreeSize(RoleID) > 0 and curhour >= 12 then
				    role.AddRoleItem(MapID, InstanceID, RoleID, 3302301, 1, -1, 8, 420)
					LingPaiLingQu_Update[Role_GuildID] = tonumber(os.date("%j"))
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	3411)		--�����񷢲��ˣ�\n    �������ڷ���ķ��踮��Χ��ʹ�ø������ٻ������踮������ͨ���û������İ��ɳ�Ա���Խ������ר�õ���˷�ͽ��������
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	21)			--��������
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif curhour >= 12 then
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	3412)		--�����񷢲��ˣ�\n    ���ı����ռ䲻�㣬������һ��������ȡ����
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	21)			--��������
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3413)		--�����񷢲��ˣ�\n    ��ÿ��ֻ����ȡһö��˷������������������ȡ����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	21)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 6 then
	    local Role_GuildID = guild.GetRoleGuildID(RoleID)
		local Master_GuilID = city.GetCityAppointedAtt(3, 2)
		local Role_ZhiWei = -1
		if Role_GuildID ~= nil and Role_GuildID ~= 4294967295 then
		    Role_ZhiWei = guild.GetRoleGuildPos(Role_GuildID, RoleID)
		end
        if Role_GuildID ~= -1 and Role_GuildID ~= 4294967295 and Role_GuildID == Master_GuilID and Role_ZhiWei == 2 then
            if fabu_date ~= tonumber(os.date("%j")) then
				fabu_date = tonumber(os.date("%j"))
				act.SetActScriptData(99, 1, 16, 3)
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3414)		--�����񷢲��ˣ�\n    ���Ѿ��ɹ���������������֮���������������Ҷ������ڴ˴���ȡ�������齱������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	21)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100128)    --�������������ʩ���ݣ�֮����������18:00-21:00������ҿ���ǰ����ս�����ڳ��񷢲��˴���ȡ���齱����
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3415)		--�����񷢲��ˣ�\n    ������ÿ��ֻ�ܷ���һ�Σ��������Ѿ��������������ˡ���
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	21)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
            end
		end
	end

end

aux.RegisterCreatureEvent(4900706, 7, "chengwufaburen")

function I3302301_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local ID = map.MapCreateColCreature(3017298383, -1, 4900707, x, y ,z)
	local Role_GuildID = guild.GetRoleGuildID(TargetID)
	cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, Role_GuildID)
	bangpaihuwei_instance[ID] = Role_GuildID
	map.CreateInstance(2092185908, Role_GuildID)
	fengxiangjiaofei_man[Role_GuildID] = 0
    fengxiangjiaofei_kill[Role_GuildID]  = 0
	--map.MapCreateColCreature(2092185908, Role_GuildID, 4900708, 1, 1 ,1)
    local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3416) --�����ڷ��衾xxx��xxx���ٻ��˷��踮����������ǰȥ������ɻ��ȡ���飡
	msg.AddMsgEvent(MsgID, 9, x)
	msg.AddMsgEvent(MsgID, 9, z)
	msg.DispatchGuildMsgEvent(MsgID, Role_GuildID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 3416)  --�����ڷ��衾xxx��xxx���ٻ��˷��踮����������ǰȥ������ɻ��ȡ���飡
	msg.AddMsgEvent(MsgID, 9, x)
	msg.AddMsgEvent(MsgID, 9, z)
	msg.DispatchGuildMsgEvent(MsgID, Role_GuildID)
end

function I3302301_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if MapID ~= 3017298383 or x > 1150 or x < 629 or z > 2400 or z < 1855 then
	    bRet = 32
	end
	if guild.GetRoleGuildID(TargetID) == nil or guild.GetRoleGuildID(TargetID) == 4294967295 or guild.GetRoleGuildPos(guild.GetRoleGuildID(TargetID), TargetID) ~= 2 then
	    bRet = 32
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3302301, 1, "I3302301_GiftBag")
aux.RegisterItemEvent(3302301, 0, "I3302301_CanUseGiftBag")

function a09_PlayerEnter(MapID, InstanceID, RoleID)
	fengxiangjiaofei_man[InstanceID] = fengxiangjiaofei_man[InstanceID] + 1
    --fengxiangjiaofei_kill[Role_GuildID]  = 0
    --map.SetMapScriptData(MapID, InstanceID, 1, 0, map.GetMapScriptData(MapID, InstanceID, 1, 0)+1)
end

--ע�������ͼ�¼�
aux.RegisterMapEvent("a09", 2, "a09_PlayerEnter")

--����뿪�����
function a09_PlayerLeave(MapID, InstanceID, RoleID)
    if fengxiangjiaofei_man[InstanceID] > 0 then
	   fengxiangjiaofei_man[InstanceID] = fengxiangjiaofei_man[InstanceID] - 1
	   -- map.SetMapScriptData(MapID, InstanceID, 1, 0, map.GetMapScriptData(MapID, InstanceID, 1, 0)-1)
	end
end

--ע���뿪���ͼ�¼�
aux.RegisterMapEvent("a09", 3, "a09_PlayerLeave")

--�ҵ�����ʵĸ���ʵ��
function a09_GetOnePerfectMap(MapID, InstanceID, RoleID)
	local Role_GuildID = guild.GetRoleGuildID(RoleID)
	if Role_GuildID ~= nil and Role_GuildID ~= 4294967295 then
	    return Role_GuildID
	else
	    return 0
	end
end

aux.RegisterMapEvent("a09",  14, "a09_GetOnePerfectMap")

--��������
function fengxiangfeilei_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    --map.SetMapScriptData(MapID, InstanceID, 1, 1, map.GetMapScriptData(MapID, InstanceID, 1, 1)+1)
	fengxiangjiaofei_kill[InstanceID]  = fengxiangjiaofei_kill[InstanceID] + 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 3417)  --�������Ѿ���ɱ������� XXX/1500
	msg.AddMsgEvent(MsgID, 9, fengxiangjiaofei_kill[InstanceID])
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(1534160, 4, "fengxiangfeilei_OnDie")
aux.RegisterCreatureEvent(1534161, 4, "fengxiangfeilei_OnDie")

function N4900707_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local guildID1 = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
	local Role_GuildID = guild.GetRoleGuildID(RoleID)
	if MapID == 3017298383 then
		if TalkIndex == -1 then
			if guildID1 == Role_GuildID then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3418)		--�����踮������\n    ֻ��xxx���ɵ��˲���ͨ�����˽������ݵ㡣��
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 19,  guildID1)
				msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	3419)			--�������ͽ�ݵ㡱
				msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3420)		--�����踮������\n    ֻ��xxx���ɵ��˲���ͨ�����˽������ݵ㡣��
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 19,  guildID1)
				msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	21)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 4 then
			if guildID1 == Role_GuildID and fengxiangjiaofei_man[guildID1] < 50 then
			    role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2092185908, ZhongZhiChengCheng_Point[1].x, ZhongZhiChengCheng_Point[1].y, ZhongZhiChengCheng_Point[1].z)
				--role.RoleGotoArena(MapID, InstanceID, RoleID, 2092185908, Role_GuildID, ZhongZhiChengCheng_Point[1].x, ZhongZhiChengCheng_Point[1].y, ZhongZhiChengCheng_Point[1].z)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3421)		--�����踮������\n    �ø�������������50�����ͬʱ���룬��ȴ���������Ƴ����ٳ��Խ��븱������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	21)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	else
	    if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	3422)		--�����踮������\n    ֻҪɱ��1500���������С���������е������˶�������������ȡ�����ľ��顣\n    �Ѿ���ɱXXX/1500����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,   fengxiangjiaofei_kill[InstanceID])
			if  fengxiangjiaofei_kill[InstanceID] >= 1500 then
			    msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	3423)			--����ȡ�������������
			end
			msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	3424)			--���뿪����ݵ㡱
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 5 then
            if Role_GuildID ~= nil and Role_GuildID ~= 4294967295 and math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["FengXiangXiaoXiao_Update"])/100) ~= tonumber(os.date("%j")) or role.GetRoleScriptData(RoleID, 1, RoleDataType["FengXiangXiaoXiao_Update"])%100 < tonumber(os.date("%H")) or role.GetRoleScriptData(RoleID, 1, RoleDataType["FengXiangXiaoXiao_Update"])%100 - tonumber(os.date("%H")) >= 2 then
				if role.GetBagFreeSize(RoleID) < 1 then
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	3451)		--�����踮������\n    ����ȷ����������һ�����ϵĿո�������ȡ����
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	21)			--��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
                    if guild.GetGuildAtt(Role_GuildID) <= 2 then
					    role.AddRoleItem(MapID, InstanceID, RoleID, 1360133, 3, -1, 8, 420)
					else
					    role.AddRoleItem(MapID, InstanceID, RoleID, 1360133, 6, -1, 8, 420)
					end
					local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
					role.AddRoleExp(MapID, InstanceID, RoleID, math.floor(activity_newexp[rolelevel]/4*3))
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FengXiangXiaoXiao_Update"], tonumber(os.date("%j"))*100+tonumber(os.date("%H")))
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	3425)		--�����踮������\n    ֻҪɱ��1500���������С���������е������˶�������������ȡ�����ľ��顣\n    �Ѿ���ɱXXX/1500����
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 9,   fengxiangjiaofei_kill[InstanceID])
					msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	3426)			--���뿪����ݵ㡱
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3427)		--�����踮������\n    �������Ѿ���ȡ��������С�Ļ�����ˣ������´�������ȡ����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	21)			--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 6 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017298383, 923, 10000, 2007)
		end
	end
end

aux.RegisterCreatureEvent(4900707, 7, "N4900707_OnTalk")

function c4900707_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
   -- if MapID == 3017298383 then
        if bangpaihuwei_instance[TargetID] ~= nil then
		   map.DeleteInstance(2092185908, bangpaihuwei_instance[TargetID])
		   fengxiangjiaofei_kill[InstanceID] = nil
		   fengxiangjiaofei_man[InstanceID] = nil
		   bangpaihuwei_instance[TargetID] = nil
		end
	--end
end

aux.RegisterCreatureEvent(4900707, 13, "c4900707_OnDisappear")

--����ڻ�е��ߺ�����
function a09_CanTakeOverWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	return 3017298383, 923, 10000, 2007
end

aux.RegisterMapEvent("a09",  12, "a09_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function a09_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3017298383, 923, 10000, 2007
end

aux.RegisterMapEvent("a09",  13, "a09_GetExporMapAndCoord")

--�������Ƿ��ܹ���������
function a09_CanEnter(MapID, InstanceID, RoleID)
    local Role_GuildID = guild.GetRoleGuildID(RoleID)
	if Role_GuildID ~= 4294967295 then
		return 1
	else
	    return 0
	end
end

aux.RegisterMapEvent("a09",  15, "a09_CanEnter")
