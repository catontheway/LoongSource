--�̶���������

--����������ȫ�ֱ�������
Act4_Gan_Num = 0     --�յ����������н�����α�鱨����
Act4_Dead_Creature_Num = 0  --����Ԯ������ɱ��
Act4_Forage = 0	--����ռ��������������


--ʱ���¼��Ĵ���ʱ��
act_Time = {900, 1800, 2700}

--ˢ��50ֻ����ʿ��
function FengHuo_CreateCreature()
	for i=1, 50 do
		local TypeID = math.random(1003158,1003160)
		local randPoint = math.random(1, 100)
		local x = map_list[3017298127].map[randPoint].x
		local y = map_list[3017298127].map[randPoint].y
		local z = map_list[3017298127].map[randPoint].z

		--ˢ����Ӧ����
		map.MapCreateCreature(3017298127, -1, TypeID, x, y, z)
	end
end

--���ʼ�¼�
function act4_OnStart(actID)
	--ˢ��50ֻ����ʿ��
	FengHuo_CreateCreature()
	--ȫ���㲥���ʼ
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 14)
	msg.DispatchMapMsgEvent(MsgID, 3017298127, -1)

end

function act4_OnInit(actID)
	--��ʼ���ʱ���¼��Ĵ���ʱ��
	act.InitEventTime("act_Time", 3, actID)
	Act4_Forage = 0
end

function act4_OnTimer(actID)
	--ˢ��50ֻ����ʿ��
	FengHuo_CreateCreature()
end

--���������Ƿ����
function I2610159_CanUesItem(MapID, InstanceID, TypeID, TargetID)
	local bIgnore = false
	local bStart = false

	--�������Ƿ񱳰�����
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		return 40, bIgnore
	end

	--����Ƿ�ʼ
	for i=5, 7 do
		bStart = act.GetActIsStart(i)
		if bStart then
			break
		end
	end

	--�����Ѿ���ʼ�������Ƿ��Ѿ��ӹ�����
	if bStart then
		for k=20001, 20003 do
			if role.IsRoleHaveQuest(MapID, InstanceID, TargetID, k) then
				--��ʾ����Ѿ���ȡ��������
				return 31, bIgnore
			end
		end
	else
		--��ʾ���ֻ���ڻ��ʹ��
		return 42, bIgnore
	end

	return 0, bIgnore
end

--ʹ���������ţ������ȡӪ����ʿ���ջپ������յ�����
function I2610159_UesItem(MapID, InstanceID, TypeID, TargetID)
	local QuestID = math.random(20001, 20003)
	if QuestID == 20001 then
		role.AddQuest(TargetID, 20001)

		--�����ȡ����������
		local Index = {}
			local i = 1
		while i ~=4 do
			local RandIndex = math.random(1, map_trigger[3017298127].triggerNum)
			Index[i] = map_trigger_m02[RandIndex]

			for k=1, i-1 do
				if Index[k] == Index[i] then
					i = i - 1
				end
			end
			i = i + 1
		end

		role.SetQuestScriptData(TargetID, 20001, 6, 1, Index[1], 2, Index[2], 3, Index[3])

	elseif QuestID == 20002 then

		role.AddQuest(TargetID, 20002)

	elseif QuestID == 20003 then

		role.AddQuest(TargetID, 20003)

	end
end

--[[ע��û������¼�
aux.RegisterActEvent(5, 2, "act4_OnStart")
aux.RegisterActEvent(5, 0, "act4_OnInit")
aux.RegisterActEvent(5, 1, "act4_OnTimer")
aux.RegisterActEvent(6, 2, "act4_OnStart")
aux.RegisterActEvent(6, 0, "act4_OnInit")
aux.RegisterActEvent(6, 1, "act4_OnTimer")
aux.RegisterActEvent(7, 2, "act4_OnStart")
aux.RegisterActEvent(7, 0, "act4_OnInit")
aux.RegisterActEvent(7, 1, "act4_OnTimer")
aux.RegisterItemEvent(2610159, 0, "I2610159_CanUesItem")
aux.RegisterItemEvent(2610159, 1, "I2610159_UesItem")]]

