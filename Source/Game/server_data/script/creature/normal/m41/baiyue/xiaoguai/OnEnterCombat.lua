-- �������ս������ʾ��
function baiyue_xiaoguai_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ5�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008012, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008044, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008045, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008058, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008059, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008060, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008061, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008064, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008066, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008067, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008068, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008069, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008070, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008071, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008072, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008073, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008075, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008076, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008077, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008078, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008079, 2, "baiyue_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1008080, 2, "baiyue_xiaoguai_OnEnterCombat")

-- �������ս������
function c1008007_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40025)
       end
	       --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008007, 2, "c1008007_OnEnterCombat")

-- �������ս������
function c1008062_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40041)
       end
	       --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008062, 2, "c1008062_OnEnterCombat")

-- �������ս������
function c1008063_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40042)
       end
	       --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008063, 2, "c1008063_OnEnterCombat")

-- �������ս������
function c1008065_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40044)
       end
	       --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008065, 2, "c1008065_OnEnterCombat")

-- �������ս������
function c1008090_OnEnterCombat(MapID, InstanceID, CreatureID)
          local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
         cre.MonsterSay(MapID, InstanceID, CreatureID, 40045)
       end
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008090, 2, "c1008090_OnEnterCombat")
